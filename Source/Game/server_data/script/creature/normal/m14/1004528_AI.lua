--���ﺰ���ű�/ʾ���ű�

-- �������ս������
function c1004528_OnEnterCombat(MapID, InstanceID, CreatureID)

             cre.MonsterSay(MapID, InstanceID, CreatureID, 10061)
	--�õ�����ĵ�ǰĿ��
    local TargetID = cre.GetCreatureCurTargetID(MapID, InstanceID, CreatureID)
    --ͬ����޸���ΧΪ8�����ӣ��߶�Ϊ20���ѷ�����
    g_SynCreatureEnmity(MapID, InstanceID, CreatureID, TargetID, 1, 10, 20)
end

aux.RegisterCreatureEvent(1004528, 2, "c1004528_OnEnterCombat")

--������������
function c1004528_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)

	cre.MonsterSay(MapID, InstanceID, TargetID, 10063)
	Increase_shili(MapID, InstanceID, RoleID, 1, 2, "jinjieshili")

	local Boss_num = map.GetMapScriptData(MapID,InstanceID,1,10)+1000
	map.SetMapScriptData(MapID,InstanceID,1,10,Boss_num)

	if Boss_num==1111 then
		local HuanJiao_ID=map.GetMapScriptData(MapID,InstanceID,1,11)
		unit.CancelBuff(MapID, InstanceID, HuanJiao_ID, 9422601)
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 101, 1737)	--��а����ýǵ��޵�״̬�ѽ����
		msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID)
	end

end

aux.RegisterCreatureEvent(1004528, 4, "c1004528_OnDie")


