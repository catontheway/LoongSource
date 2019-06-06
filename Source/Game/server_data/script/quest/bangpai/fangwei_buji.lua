--[[city.ChangeCityDefence(MapID,InstanceID,ChangeVal)         ChangeVal�����ɸ�

city.GetCityAppointedAttByMapID(MapID,InstanceID,Type)


Type Ϊ�����ֵ
	ECAT_CityID = 0,			// ����ID
	ECAT_ClanType,	1		// ��������
	ECAT_Holder,	2		// ͳ�ΰ���ID
	ECAT_Defence,	3		// ����
	ECAT_EudemonTally,	4	// �ػ������϶�
	ECAT_TaxRate,	5		// ˰��
	ECAT_TaxRateTime,	6	// ˰�ʸ���ʱ��
	ECAT_Taxation,			// ˰��
	ECAT_Prolificacy,		// ������
	ECAT_FundOutput,		// �ʽ����
	ECAT_MaterialOutput,	// �ʲĲ���
	ECAT_ItemOutput,		// ��Ʒ����

]]



function q30006_OnCheckAccept(MapID, InstanceID, QuestID, RoleID, NPCID)
	--�ж�����Ƿ��Ѽ������
	local GuildID = guild.GetRoleGuildID(RoleID)
	if GuildID==4294967295 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 26, 143)
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
		return 0
	end

	--�жϵ�ǰ���е�ռ�����ID ��������ڰ���ID�Ƿ���ͬ
	local curguild = city.GetCityAppointedAttByMapID(MapID,InstanceID,2)
	if curguild ~= GuildID then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 26, 1027)--�����İ��ɲ�û��ռ���������С�
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
		return 0
	end

	--�ж���Ұ���ְλ
	local GuildPos=guild.GetRoleGuildPos(GuildID, RoleID)
	if (GuildPos~=2) and (GuildPos~=3) and (GuildPos~=12) and (GuildPos~=13) and (GuildPos~=14) then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 26, 145)
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
		return 0
	end

	--�жϱ��������ȡ�����Ƿ�����
	--�õ���ǰ������������
	local LoopNum = role.GetRoleScriptData(RoleID, 1, RoleDataType["fangwei"])
	local UpdateTime = role.GetRoleScriptData(RoleID, 1, RoleDataType["fangwei_Update"])

	--�ܻ���Ϊ0ʱ�������ܻ���Ϊ��һ��
	if 0 == LoopNum then
		LoopNum = 1
		role.SetRoleScriptData(RoleID, 1, RoleDataType["fangwei"], LoopNum)
	end

	--��������ϴθ������ڣ����뵱ǰ���ڲ�һ�£��������ܻ�������������
	local CurTime = tonumber(os.date("%j"))
	if CurTime~=UpdateTime then
		role.SetRoleScriptData(RoleID, 1, RoleDataType["fangwei"], 1)
		role.SetRoleScriptData(RoleID, 1, RoleDataType["fangwei_Update"], CurTime)
	end

	--�������ѽ�ȡ50�Σ����޷��ٽ�ȡ
	if LoopNum > 50 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 26, 147)
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
		return 0
	end
	return 1
end

--�������
function q30006_OnComplete(MapID, InstanceID, QuestID, RoleID, NPCID)


	--���з���ֵ��10
	city.ChangeCityDefence(MapID,InstanceID,10)

	local exp_award = {}
	exp_award[3] = 470
	exp_award[4] = 630
	exp_award[5] = 890
	exp_award[6] = 1170
	exp_award[7] = 1590
	exp_award[8] = 2040
	exp_award[9] = 2580
	exp_award[10] = 2790

	local rolelevel = role.GetRoleLevel(MapID, InstanceID, RoleID)
	local k = math.floor(rolelevel/10)
	if k>10 then
		k = 10
	end

	--�õ���ǰ�����ܻ���
	local LoopNum = role.GetRoleScriptData(RoleID, 1, RoleDataType["fangwei"])

	--���㻷��
	local LoopNum_Ring = LoopNum % 10
	if LoopNum_Ring == 0 then
		LoopNum_Ring = 10
	end

	--��������
	local LoopNum_Round = (LoopNum - LoopNum_Ring) /10 + 1
	--���㾭�齱��
	local Exp = exp_award[k]*(1+(LoopNum_Ring-1)*0.3)*(1 + LoopNum_Round*0.1)
	--������Ҿ���
	role.AddRoleExp(MapID, InstanceID, RoleID, Exp)

	--��50����������300
	if (LoopNum==50) then
		local GuildID = guild.GetRoleGuildID(RoleID)
		guild.ModifyRoleContribute(GuildID, RoleID, 300, 102)
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 100, 1028)--"�����Ա�������������ң�������X�������ͳ���ṩ�����������з���ֵ�������X����ɹ��׽�����"
		msg.AddMsgEvent(MsgID, 2, RoleID)
		msg.AddMsgEvent(MsgID, 9, LoopNum)
		msg.AddMsgEvent(MsgID, 9, 300)
		msg.DispatchGuildMsgEvent(MsgID,GuildID)

	elseif (LoopNum_Ring==10) then	--��10����������100
		local GuildID = guild.GetRoleGuildID(RoleID)
		guild.ModifyRoleContribute(GuildID, RoleID, 100, 102)
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 100, 1028)--"�����Ա�������������ң�������X�������ͳ���ṩ�����������з���ֵ�������X����ɹ��׽�����"
		msg.AddMsgEvent(MsgID, 2, RoleID)
		msg.AddMsgEvent(MsgID, 9, LoopNum)
		msg.AddMsgEvent(MsgID, 9, 100)
		msg.DispatchGuildMsgEvent(MsgID,GuildID)

	end
	--����������һ
	role.SetRoleScriptData(RoleID, 1, RoleDataType["fangwei"], LoopNum+1)

end


function q30006_OnCheckComplete(MapID, InstanceID, QuestID, RoleID, NPCID)

	--�ж�����Ƿ��Ѽ������
	local GuildID = guild.GetRoleGuildID(RoleID)
	if GuildID==4294967295 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 26, 143)
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
		return 0
	end

	--�жϵ�ǰ���е�ռ�����ID ��������ڰ���ID�Ƿ���ͬ
	local curguild = city.GetCityAppointedAttByMapID(MapID,InstanceID,2)
	if curguild ~= GuildID then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 26, 1027)--�����İ��ɲ�û��ռ���������С�
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
		return 0
	end
	return 1
end

aux.RegisterQuestEvent(30006, 4, "q30006_OnCheckAccept")
aux.RegisterQuestEvent(30006, 1, "q30006_OnComplete")
aux.RegisterQuestEvent(30006, 5, "q30006_OnCheckComplete")

