--���ﺰ��/ʾ���ű�

-- �������ս������
function c1003121_OnEnterCombat(MapID, InstanceID, CreatureID)
       local temp = math.random(1,100)
	if temp>=1 and temp<=15 then
		--15%���ʺ���
             cre.MonsterSay(MapID, InstanceID, CreatureID, 80)
	end
	   --�õ�����ĵ�ǰĿ��
       local TargetID = cre.GetCreatureCurTargetID(MapID, InstanceID, CreatureID)
       --ͬ����޸���ΧΪ8�����ӣ��߶�Ϊ20���ѷ�����
       g_SynCreatureEnmity(MapID, InstanceID, CreatureID, TargetID, 1, 10, 20)
end

aux.RegisterCreatureEvent(1003121, 2, "c1003121_OnEnterCombat")

--��������
function c1003121_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)
	Increase_shili(MapID, InstanceID, RoleID, 1, 1, "jinjieshili")
end

aux.RegisterCreatureEvent(1003121, 4, "c1003121_OnDie")
