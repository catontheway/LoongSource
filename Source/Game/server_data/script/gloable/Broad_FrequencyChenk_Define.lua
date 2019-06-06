--���ڼ��㲥����Ƶ�ʲ���ͨ�����󲥷Ź㲥��ȫ�ֺ������������ݶ���



-----------DisRange���㲥��Ͷ�ͷ�Χ���Ŀ�ѡֵ------------------
	  DPR_ALL		=	1   --ȫ���㲥
	  DPR_INMAP		=	2   --�����㲥
	  DPR_ININSTANCE	=	3   --��һ�����ڲ��㲥
	  DPR_INGUILD		=	4   --���ɹ㲥
	  DPR_INROLE		=	5   --��ҽ�ɫ�㲥
	  DPR_INTEAM		=	6   --С�ӹ㲥
---------------------------------------------------------------
-----------ChenkWith����ⲥ��Ƶ�ʵĲ�����Ŀ�ѡֵ-----------
	   CFT_BROAD	=	1	--�Ե����㲥���й㲥�Ĳ���Ƶ��Ϊ�жϱ�׼
	   CFT_IDMSG	=	2	--�Ե���id_Msg��string_table�е���Ϣ�ı��Ĳ���Ƶ��Ϊ�жϱ�׼
	   CFT_ROLE	=	3	--�Ե�����ҽ�ɫ�����㲥�¼���Ƶ��Ϊ�жϱ�׼
	   CFT_ITEM	=	4	--��ĳ����Ʒ�����㲥�¼���Ƶ��Ϊ�жϱ�׼
	   CFT_NPC	=	5	--��ĳ�������NPC�����㲥�¼���Ƶ��Ϊ�жϱ�׼
	   CFT_ACT	=	6	--��ĳ����������Ĺ㲥�¼���Ƶ��Ϊ�жϱ�׼
	   CFT_QUEST	=	7	--��ĳ�������������Ĺ㲥�¼���Ƶ��Ϊ�жϱ�׼
---------------------------------------------------------------
ChenkTab = {};
ChenkTab[CFT_BROAD] = {LastTime = {}};
ChenkTab[CFT_IDMSG] = {LastTime = {}};
ChenkTab[CFT_ROLE] = {LastTime = {}};
ChenkTab[CFT_ITEM] = {LastTime = {}};
ChenkTab[CFT_NPC] = {LastTime = {}};
ChenkTab[CFT_ACT] = {LastTime = {}};
ChenkTab[CFT_QUEST] = {LastTime = {}};

--����ȫ���㲥
function DispatchBroad_ALL (MsgID, MapID, InstanceID, GuildID, RoleID)	--ȫ������
	local isSucc = 0	--�����ķ���ֵ�����ڼ�⺯����ִ��״��
	if MsgID == nil then	--�ж�MsgID�Ƿ�Ϊ��ֵ
		isSucc = -1
	else
		msg.DispatchBroadcast(MsgID,-1,-1,-1)
		isSucc = 1	--���ųɹ�
	end
	return isSucc
end
--���ų����㲥
function DispatchBroad_INMAP(MsgID, MapID, InstanceID, GuildID, RoleID)	--��������
	local isSucc = 0	--�����ķ���ֵ�����ڼ�⺯����ִ��״��
	if MsgID == nil then
		isSucc = -1
	else
		msg.DispatchBroadcast(MsgID,MapID,-1,-1)
		isSucc = 1		
		if MapID == 4294967295 then	--����MapIDΪ-1�����²�����ȫ���㲥���򷵻�2
			isSucc = 2
		end
	end
	return isSucc
end
--���ŵ�һ�����ڲ��㲥
function DispatchBroad_ININSTANCE(MsgID, MapID, InstanceID, GuildID, RoleID)	--��һ�����ڲ�����
	local isSucc = 0	--�����ķ���ֵ�����ڼ�⺯����ִ��״��
	if MsgID == nil then
		isSucc = -1
	else
		msg.DispatchBroadcast(MsgID,MapID,InstanceID,-1)
		isSucc = 1
		if MapID == 4294967295 or InstanceID == 4294967295 then	--����MapID��InstanceIDΪ-1�����²�����ȫ���㲥���򷵻�2
			isSucc = 2
		end
	end
	return isSucc
end
--���Ű����ڲ��㲥
function DispatchBroad_INGUILD(MsgID, MapID, InstanceID, GuildID, RoleID)	--��һ�����ڲ�����
	local isSucc = 0	--�����ķ���ֵ�����ڼ�⺯����ִ��״��
	if MsgID == nil then
		isSucc = -1
	elseif guild.GetGuildFund(GuildID) == nil then	--���GuildID�Ƿ���ã��������ã���ͨ��RoleID���»�ȡһ��GuildID
		if guild.GetRoleGuildID(RoleID) == nil then
			isSucc = -2
		else
			local guild_id = guild.GetRoleGuildID(RoleID)
			if guild_id == 4294967295 then	--����ȡ����GuildID�Ƿ�Ϊ-1����Ϊ-1��ֻ������Ҳ��Ź㲥
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
				isSucc = 2
			else
				msg.DispatchBroadcast(MsgID,-1,-1,guild_id)
				isSucc = 1
			end
		end
	else
		msg.DispatchBroadcast(MsgID,-1,-1,GuildID)
		isSucc = 1
	end
	return isSucc
end
--������ҹ㲥
function DispatchBroad_INROLE(MsgID, MapID, InstanceID, GuildID, RoleID)	--��ĳһ��ҽ�ɫ����
	local isSucc = 0	--�����ķ���ֵ�����ڼ�⺯����ִ��״��
	if MsgID == nil then
		isSucc = -1
	elseif role.GetRoleName(RoleID) == nil then
		isSucc = -2
	else
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
		isSucc = 1
	end
	return isSucc
end
--����С�ӹ㲥
function DispatchBroad_INTEAM(MsgID, MapID, InstanceID, GuildID, RoleID)	--��ĳһ���С�Ӳ���
	local isSucc = 0	--�����ķ���ֵ�����ڼ�⺯����ִ��״��
	if MsgID == nil then
		isSucc = -1
	elseif role.IsRoleHaveTeam(MapID, InstanceID, RoleID) == nil then	--����Ƿ�ɻ�����С��ID���������򷵻�-2
		isSucc = -2
	else 
		local TeamID = role.IsRoleHaveTeam(MapID, InstanceID, RoleID)	--��ȡ���С��ID
		if TeamID == 4294967295 then					--�����û��С�ӣ��򲥷ŵ��˹㲥���ҷ���2
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
			isSucc = 2
		else
			local Role = {}
			Role[1], Role[2], Role[3], Role[4], Role[5], Role[6] = role.GetRoleTeamMemID(TeamID)
			for i=1,6 do
				if Role[i] and Role[i] ~= 4294967295 then
					msg.DispatchRoleMsgEvent(Role[i], MsgID)
				end
			end
			isSucc = 1
		end
	end
	return isSucc
end

--�㲥���ź�������Ӧ��ͬ��������
RunBroadFuncTab = {};
RunBroadFuncTab[DPR_ALL] = DispatchBroad_ALL
RunBroadFuncTab[DPR_INMAP] = DispatchBroad_INMAP
RunBroadFuncTab[DPR_ININSTANCE] = DispatchBroad_ININSTANCE
RunBroadFuncTab[DPR_INGUILD] = DispatchBroad_INGUILD
RunBroadFuncTab[DPR_INROLE] = DispatchBroad_INROLE
RunBroadFuncTab[DPR_INTEAM] = DispatchBroad_INTEAM

function RunBroad(MsgID, MapID, InstanceID, GuildID, RoleID, DisRange, ChenkWith, IndexID, TimerMin)
	if MsgID == nil then return -1 end

	local CanDispath = true
	local dtime = 0
	
	if TimerMin > 0 then	
		if ChenkTab[ChenkWith].LastTime[IndexID] == nil then
			CanDispath = true
		else
			dtime = os.difftime(os.time(),ChenkTab[ChenkWith].LastTime[IndexID])
			if dtime>TimerMin*60 then
				CanDispath = true
			else
				CanDispath = false
			end
		end
	end

	if CanDispath then
		if RunBroadFuncTab[DisRange](MsgID, MapID, InstanceID, GuildID, RoleID)>0 then
			ChenkTab[ChenkWith].LastTime[IndexID] = os.time()
		else
			return -1
		end
	end
end

--[[
	RunBroad������DispatchBroadcast��DispatchRoleMsgEvent���������˷�װ�����ڲ��Ź㲥ǰ�����˼��㲥����Ƶ�ʵĹ��ܣ������ųɹ��򷵻�ֵΪʵ�ʲ��ŷ�Χ�Ķ�Ӧֵ������Ϊ-1

	RunBroad������ľ��庬�壺

	MsgID����Ҫ���ŵĹ㲥������˾��Ӧ����ע�Ტ���������ӵľ����������������-1��

	MapID���㲥��ҪͶ�͵ĳ���CRCֵ����Ҫ����ȫ���㲥�������ҽ�ɫ���ţ���˴���Ϊ-1��

	InstanceID����������ID��Ĭ��ֵΪ-1

	GuildID���㲥��ҪͶ�͵İ���ID�������벥�Ű��ɷ�Χ�㲥����˴���Ϊ-1

	RoleID����ҽ�ɫID�����㲥��Ͷ�ͷ�ΧΪ������ҽ�ɫ����ʱ������Ҫ�ø�ֵ�����ƾ�����ȡ�㲥����ҽ�ɫ��Ĭ��ֵΪ-1

	DisRange���㲥��Ͷ�ͷ�Χ����ѡֵ
		  DPR_ALL		=	0   --ȫ���㲥
		  DPR_INMAP		=	1   --�����㲥
		  DPR_ININSTANCE	=	2   --��һ�����ڲ��㲥
		  DPR_INGUILD		=	3   --���ɹ㲥
		  DPR_INROLE		=	4   --��ҽ�ɫ�㲥
		  DPR_INTEAM		=	5   --С�ӹ㲥

	ChenkWith�����ڼ��㲥����Ƶ�ʵĲ������ѡֵΪ��
		   CFT_BROAD	=	0	--�Ե����㲥���й㲥�Ĳ���Ƶ��Ϊ�жϱ�׼
		   CFT_IDMSG	=	1	--�Ե���id_Msg��string_table�е���Ϣ�ı��Ĳ���Ƶ��Ϊ�жϱ�׼
		   CFT_ROLE	=	2	--�Ե�����ҽ�ɫ�����㲥�¼���Ƶ��Ϊ�жϱ�׼
		   CFT_ITEM	=	3	--��ĳ����Ʒ�����㲥�¼���Ƶ��Ϊ�жϱ�׼
		   CFT_NPC	=	4	--��ĳ�������NPC�����㲥�¼���Ƶ��Ϊ�жϱ�׼
		   CFT_ACT	=	5	--��ĳ����������Ĺ㲥�¼���Ƶ��Ϊ�жϱ�׼
		   CFT_QUEST	=	6	--��ĳ�������������Ĺ㲥�¼���Ƶ��Ϊ�жϱ�׼

	IndexID����ChenkWithֵһһ��Ӧ�Ĳ������������ֵ�������Ӧ��ϵ���£�
		��ChenkWith��ֵΪ0��CFT_BROAD����IndexID�ɴ���ù㲥��broadcast_list.xml�е�ID
		��ChenkWith��ֵΪ1��CFT_IDMSG����IndexID�ɴ���ù㲥��id_msg.xml�е�ID
		��ChenkWith��ֵΪ2��CFT_ROLE����IndexID�ɴ����Ӧ��ҽ�ɫ��RoleID
		��ChenkWith��ֵΪ3��CFT_ITEM����IndexID�ɴ����Ӧ��Ʒ��TypeID
		��ChenkWith��ֵΪ4��CFT_NPC����IndexID�ɴ����Ӧ�����NPC��TypeID
		��ChenkWith��ֵΪ5��CFT_ACT����IndexID�ɴ����Ӧ��ĻID
		��ChenkWith��ֵΪ6��CFT_QUEST����IndexID�ɴ����Ӧ���������ID

	TimerMin�����ʱ�䣨�֣�����Ϊ0���޼������
]]